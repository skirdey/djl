/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance
 * with the License. A copy of the License is located at
 *
 * http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
 * and limitations under the License.
 */
package ai.djl.basicdataset;

import ai.djl.Model;
import ai.djl.ndarray.types.Shape;
import ai.djl.nn.Activation;
import ai.djl.repository.MRL;
import ai.djl.training.DefaultTrainingConfig;
import ai.djl.training.Trainer;
import ai.djl.training.TrainingConfig;
import ai.djl.training.dataset.Batch;
import ai.djl.training.dataset.Dataset;
import ai.djl.training.initializer.Initializer;
import java.io.IOException;
import java.util.Iterator;
import org.testng.Assert;
import org.testng.annotations.Test;

public class CocoTest {

    @Test
    public void testCocoRemote() throws IOException {
        CocoDetection coco =
                new CocoDetectionUnitTest(
                        new CocoDetection.Builder()
                                .setUsage(Dataset.Usage.TEST)
                                .setRandomSampling(1));
        coco.prepare();
        TrainingConfig config = new DefaultTrainingConfig(Initializer.ONES);
        try (Model model = Model.newInstance()) {
            model.setBlock(Activation.IDENTITY_BLOCK);
            try (Trainer trainer = model.newTrainer(config)) {
                Iterator<Batch> ds = trainer.iterateDataset(coco).iterator();
                Batch batch = ds.next();
                Assert.assertEquals(
                        batch.getData().singletonOrThrow().getShape(), new Shape(1, 3, 426, 640));
                Assert.assertEquals(
                        batch.getLabels().singletonOrThrow().getShape(), new Shape(1, 20, 5));
            }
        }
    }

    private static final class CocoDetectionUnitTest extends CocoDetection {

        CocoDetectionUnitTest(CocoDetection.Builder builder) {
            super(builder);
        }

        /** {@inheritDoc} */
        @Override
        public MRL getMrl() {
            return new MRL(MRL.Dataset.CV, BasicDatasets.GROUP_ID, "coco-unittest");
        }
    }
}
